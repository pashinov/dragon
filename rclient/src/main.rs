mod falcon;
mod service;
mod connection;

fn main() {
    let mut settings = config::Config::default();
    settings.merge(config::File::with_name("/etc/falcon/rclient.yaml")).unwrap();

    let srv = service::Service::new(&settings);
    srv.start();
}
