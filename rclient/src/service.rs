use std::time::Duration;
use std::sync::mpsc::{Receiver, RecvTimeoutError};

use config::Config;
use protobuf::Message;

use crate::falcon;

pub struct Service  {
    ctx: bool,
    settings: Config
}

impl Service  {
    pub fn new(settings: Config) -> Self {
        Service { ctx: true, settings }
    }

    pub fn run(&mut self, rx: Receiver<i32>) -> () {
        info!("Running service...");

        let service_zmq_url = self.settings.get_str("service.zmq.url").unwrap();
        let service_zmq_timeout= self.settings.get_int("service.zmq.timeout").unwrap();
        let router_zmq_url= self.settings.get_str("router.zmq.url").unwrap();

        let service_channel_timeout_secs = self.settings.get_int("service.channel.timeout.secs").unwrap() as u64;
        let service_channel_timeout_nenos = self.settings.get_int("service.channel.timeout.nanos").unwrap() as u32;
        let service_channel_timeout = Duration::new(service_channel_timeout_secs, service_channel_timeout_nenos);

        let zctx = zmq::Context::new();
        let socket = zctx.socket(zmq::ROUTER).unwrap();

        socket.bind(&service_zmq_url).unwrap();

        if !self.register_service(&zctx, &service_zmq_url, &router_zmq_url) {
            panic!("rclient: Unable to register with {}", router_zmq_url);
        }

        let mut poll_items = vec![socket.as_poll_item(zmq::POLLIN)];

        while self.ctx {
            zmq::poll(&mut poll_items, service_zmq_timeout).unwrap();

            if poll_items[0].get_revents() == zmq::POLLIN {
                let mut zmsg = socket.recv_multipart(0).unwrap();

                let request: falcon::request_t = protobuf::parse_from_bytes(&zmsg[zmsg.len() - 1]).unwrap();
                let mut response = falcon::response_t::new();

                response.set_svc_id(request.svc_id);
                response.set_cmd_id(request.cmd_id);
                response.set_branch(request.branch);
                response.set_status(0);
                response.set_resp("rclient: ".to_string() + &request.payload);

                // replace last element that contains data with created response
                zmsg.remove(zmsg.len() - 1);
                zmsg.push(response.write_to_bytes().unwrap());

                socket.send_multipart(&zmsg, 0).unwrap();
            }

            match rx.recv_timeout(service_channel_timeout) {
                Ok(message) => {
                    match message {
                        signal_hook::SIGINT => { self.ctx = false; info!("SIGINT was handled"); },
                        signal_hook::SIGTERM => { self.ctx = false; info!("SIGTERM was handled"); },
                        _ => unreachable!(),
                    }
                },
                Err(RecvTimeoutError::Timeout) => {},
                Err(RecvTimeoutError::Disconnected) => { warn!("The channel was disconnected") },
            }
        }

        if !self.deregister_service(&zctx, &service_zmq_url, &router_zmq_url) {
            panic!("rclient: Unable to deregister from {}", router_zmq_url);
        }

        drop(socket);

        info!("Stopping the service...");
    }

    fn register_service(&self, zctx: &zmq::Context, reg_url: &String, dest_url: &String) -> bool {
        let mut rval = false;

        let mut reg_data = falcon::reg_data_t::new();
        reg_data.set_svc_url(reg_url.clone());

        let mut request = falcon::request_t::new();

        request.set_svc_id(falcon::service_id::RCLIENT_ID);
        request.set_cmd_id(falcon::command_id::CMD_REG_ID);
        request.set_payload(String::from_utf8(reg_data.write_to_bytes().unwrap()).unwrap());

        let socket = zctx.socket(zmq::REQ).unwrap();
        socket.connect(&dest_url).unwrap();

        socket.send(&request.write_to_bytes().unwrap(), 0).unwrap();

        let mut msg = zmq::Message::new();
        socket.recv(&mut msg, 0).unwrap();

        let response: falcon::response_t = protobuf::parse_from_bytes(&msg).unwrap();
        if response.status == 0 {
            rval = true;
        }

        rval
    }

    fn deregister_service(&self, zctx: &zmq::Context, dereg_url: &String, dest_url: &String) -> bool {
        let mut rval = false;

        let mut dereg_data = falcon::dereg_data_t::new();
        dereg_data.set_svc_url(dereg_url.clone());

        let mut request = falcon::request_t::new();

        request.set_svc_id(falcon::service_id::RCLIENT_ID);
        request.set_cmd_id(falcon::command_id::CMD_DEREG_ID);
        request.set_payload(String::from_utf8(dereg_data.write_to_bytes().unwrap()).unwrap());

        let socket = zctx.socket(zmq::REQ).unwrap();
        socket.connect(&dest_url).unwrap();

        socket.send(&request.write_to_bytes().unwrap(), 0).unwrap();

        let mut msg = zmq::Message::new();
        socket.recv(&mut msg, 0).unwrap();

        let response: falcon::response_t = protobuf::parse_from_bytes(&msg).unwrap();
        if response.status == 0 {
            rval = true;
        }

        rval
    }
}
