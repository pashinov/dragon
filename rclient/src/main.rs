mod falcon;
mod service;

use std::thread;
use std::sync::mpsc;
use std::sync::mpsc::{Sender, Receiver};

use daemonize::Daemonize;
use signal_hook::iterator::Signals;

fn main() {
    let mut settings = config::Config::default();
    settings.merge(config::File::with_name("/etc/falcon/rclient.yaml")).unwrap();

    let daemonize = Daemonize::new()
        .pid_file(settings.get_str("service.daemon.pidfile").unwrap())
        .working_directory("/")
        .umask(0o777)
        .privileged_action(|| "Executed before drop privileges");

    match daemonize.start() {
        Ok(_) => {
            let (tx, rx): (Sender<i32>, Receiver<i32>) = mpsc::channel();

            let mut srv = service::Service::new(settings);
            let thread = thread::spawn(move || srv.run(rx));

            let signals = Signals::new(&[signal_hook::SIGINT, signal_hook::SIGTERM]).unwrap();
            for signal in &signals {
                match signal {
                    signal_hook::SIGTERM => { tx.send(signal_hook::SIGTERM).unwrap(); break },
                    signal_hook::SIGINT  => { tx.send(signal_hook::SIGINT) .unwrap(); break },
                    _ => unreachable!(),
                }
            }

            thread.join().expect_err("Thread join");
        }
        Err(e) => panic!("Error, {}", e),
    }
}
