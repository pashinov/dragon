use crate::falcon;
use protobuf::Message;

pub struct Service  {
    ctx: bool
}

impl Service  {
    pub fn new() -> Self {
        Service { ctx: true }
    }

    pub fn start(&self) -> () {
        let zctx = zmq::Context::new();
        let socket = zctx.socket(zmq::ROUTER).unwrap();

        let url = "ipc:///tmp/rclient".to_string();
        let timeout = 5000;

        socket.bind(&url).unwrap();

        let mut poll_items = vec![socket.as_poll_item(zmq::POLLIN)];

        while self.ctx {
            zmq::poll(&mut poll_items, timeout).unwrap();

            if poll_items[0].get_revents() == zmq::POLLIN {
                let mut zmsg = socket.recv_multipart(0).unwrap();

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

                socket.send_multipart(&zmsg, 0).unwrap();
            }
        }

        drop(socket);
    }

    #[allow(dead_code)]
    pub fn stop(&mut self) -> ()
    {
        self.ctx = true;
    }
}
