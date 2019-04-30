mod amsp;
mod service;

fn main() {
    let srv = service::Service::new();
    srv.start();
}
