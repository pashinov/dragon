mod falcon;
mod service;
mod connection;

fn main() {
    let srv = service::Service::new();
    srv.start();
}
