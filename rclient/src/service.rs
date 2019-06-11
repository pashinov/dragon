use protobuf::Message;
use zmq::{SocketType};

use crate::falcon;
use crate::connection::Zmq;

pub struct Service  {
    ctx: bool
}

impl Service  {
    pub fn new() -> Self {
        Service { ctx: true }
    }

    pub fn start(&self) -> () {
        let mut zmq = Zmq::new()
            .url("".to_string())
            .timeout(5000)
            .socket_type(SocketType::ROUTER)
            .finalize();

        zmq.create().unwrap();
        zmq.bind().unwrap();

        let mut poll_items = vec![zmq.get_socket().as_poll_item(zmq::POLLIN)];

        while self.ctx {
            zmq.poll(&mut poll_items).unwrap();
            if poll_items[0].get_revents() == zmq::POLLIN {
                let mut zmsg = zmq.recv_multipart().unwrap();

                let request: falcon::request_t = protobuf::parse_from_bytes(&zmsg[zmsg.len() - 1]).unwrap();
                let mut response = falcon::response_t::new();

                response.set_srv_id(request.srv_id);
                response.set_cmd_id(request.cmd_id);
                response.set_branch(request.branch);
                response.set_status(0);
                response.set_resp("rclient: Hello, World!!!".to_string());

                // replace last element that contains data with created response
                zmsg.remove(zmsg.len() - 1);
                zmsg.push(response.write_to_bytes().unwrap());

                zmq.send_multipart(&zmsg).unwrap();
            }
        }
    }

    #[allow(dead_code)]
    pub fn stop(&mut self) -> ()
    {
        self.ctx = true;
    }
}
