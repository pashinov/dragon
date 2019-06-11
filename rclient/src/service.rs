use config::Config;
use protobuf::Message;

use crate::falcon;

pub struct Service<'a>  {
    ctx: bool,
    settings: &'a Config
}

impl<'a> Service<'a>  {
    pub fn new(settings: &'a Config) -> Self {
        Service { ctx: true, settings }
    }

    pub fn start(&self) -> () {
        let zmq_url = self.settings.get_str("service.url").unwrap();
        let zmq_timeout = self.settings.get_int("service.timeout").unwrap();
        let router_url = self.settings.get_str("router.url").unwrap();

        let zctx = zmq::Context::new();
        let socket = zctx.socket(zmq::ROUTER).unwrap();

        socket.bind(&zmq_url).unwrap();

        if !self.register_service(&zctx, &zmq_url, &router_url) {
            panic!("rclient: Unable to register");
        }

        let mut poll_items = vec![socket.as_poll_item(zmq::POLLIN)];

        while self.ctx {
            zmq::poll(&mut poll_items, zmq_timeout).unwrap();

            if poll_items[0].get_revents() == zmq::POLLIN {
                let mut zmsg = socket.recv_multipart(0).unwrap();

                let request: falcon::request_t = protobuf::parse_from_bytes(&zmsg[zmsg.len() - 1]).unwrap();
                let mut response = falcon::response_t::new();

                response.set_svc_id(request.srv_id);
                response.set_cmd_id(request.cmd_id);
                response.set_branch(request.branch);
                response.set_status(0);
                response.set_resp("rclient: Hello, World!!!".to_string());

                // replace last element that contains data with created response
                zmsg.remove(zmsg.len() - 1);
                zmsg.push(response.write_to_bytes().unwrap());

                socket.send_multipart(&zmsg, 0).unwrap();
            }
        }

        if !self.deregister_service(&zctx, &zmq_url, &router_url) {
            panic!("rclient: Unable to deregister");
        }

        drop(socket);
    }

    fn register_service(&self, zctx: &zmq::Context, reg_url: &String, dest_url: &String) -> bool {
        let mut rval = false;

        let mut reg_data = falcon::reg_data_t::new();
        reg_data.set_svc_url(reg_url.clone());

        let mut request = falcon::request_t::new();

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

    #[allow(dead_code)]
    pub fn stop(&mut self) -> ()
    {
        self.ctx = true;
    }
}
