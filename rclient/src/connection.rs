use zmq::{Socket, SocketType, Context, PollItem};

pub struct Zmq {
    url:            String,
    timeout:        i64,
    socket_type:    SocketType,
    zmq_ctx:        Context,
    socket:         Option<Socket>
}

impl Zmq {
    pub fn new() -> Zmq {
        Zmq { url: String::new(), timeout: 0, socket_type: SocketType::PAIR, zmq_ctx: zmq::Context::new(), socket: None }
    }

    pub fn url(&mut self, url: String) -> &mut Zmq {
        self.url = url.clone();
        self
    }

    pub fn timeout(&mut self, timeout: i64) -> &mut Zmq {
        self.timeout = timeout;
        self
    }

    pub fn socket_type(&mut self, socket_type: SocketType) -> &mut Zmq {
        self.socket_type = socket_type;
        self
    }

    pub fn finalize(&self) -> Zmq {
        Zmq { url: self.url.clone(), timeout: self.timeout, socket_type: self.socket_type, zmq_ctx: self.zmq_ctx.clone(), socket: None }
    }

    pub fn create(&mut self) -> zmq::Result<()> {
        match self.zmq_ctx.socket(self.socket_type) {
            Ok(socket) => {
                self.socket = Some(socket);
                Ok(())
            },
            Err(err) => Err(err),
        }
    }

    pub fn bind(&self) -> zmq::Result<()> {
        match self.socket {
            Some(ref socket) => socket.bind(&self.url),
            None => Err(zmq::Error::EFAULT)
        }
    }

    pub fn poll(&self, items: &mut [PollItem]) -> zmq::Result<(i32)> {
        zmq::poll(items, self.timeout)
    }

    pub fn recv_multipart(&self) -> zmq::Result<Vec<Vec<u8>>> {
        match self.socket {
            Some(ref socket) => socket.recv_multipart(0),
            None => Err(zmq::Error::EFAULT)
        }
    }

    pub fn send_multipart(&self, msg: &Vec<Vec<u8>>) -> zmq::Result<()> {
        match self.socket {
            Some(ref socket) => socket.send_multipart(msg, 0),
            None => Err(zmq::Error::EFAULT)
        }
    }

    pub fn get_socket(&self) -> &Socket {
        match self.socket {
            Some(ref socket) => socket,
            None => panic!()
        }
    }
}
