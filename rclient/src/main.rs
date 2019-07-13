#[macro_use]
extern crate log;

mod falcon;
mod service;

use std::thread;
use std::sync::mpsc;
use std::sync::mpsc::{Sender, Receiver};
use std::str::FromStr;

use daemonize::Daemonize;
use signal_hook::iterator::Signals;

use log::LevelFilter;
use log4rs::append::file::FileAppender;
use log4rs::encode::pattern::PatternEncoder;
use log4rs::config::{Appender, Config, Root};

fn main() {
    let mut settings = config::Config::default();

    // Init configuration
    {
        settings.merge(config::File::with_name("/etc/falcon/rclient.yaml")).unwrap();
    }

    // Init logging
    {
        let log_file = settings.get_str("service.log.path").unwrap();
        let log_level_str = settings.get_str("service.log.level").unwrap();
        let log_level = LevelFilter::from_str(&log_level_str).unwrap();

        let logfile = FileAppender::builder()
            .encoder(Box::new(PatternEncoder::new("{d} [{l}] - {m}\n")))
            .build(log_file).unwrap();

        let config = Config::builder()
            .appender(Appender::builder().build("logfile", Box::new(logfile)))
            .build(Root::builder()
                .appender("logfile")
                .build(log_level)).unwrap();

        log4rs::init_config(config).unwrap();
    }

    // Create daemon instance
    let daemonize = Daemonize::new()
        .pid_file(settings.get_str("service.daemon.pidfile").unwrap())
        .working_directory("/")
        .umask(0o027)
        .privileged_action(|| info!("Starting the daemon..."));

    match daemonize.start() {
        Ok(_) => {
            let (tx, rx): (Sender<i32>, Receiver<i32>) = mpsc::channel();

            // Running main thread
            let mut srv = service::Service::new(settings);
            let thread = thread::spawn(move || srv.run(rx));

            // Running handler of signals
            let signals = Signals::new(&[signal_hook::SIGINT, signal_hook::SIGTERM]).unwrap();
            for signal in &signals {
                match signal {
                    signal_hook::SIGINT => {
                        info!("SIGINT was received");
                        tx.send(signal_hook::SIGINT).unwrap();
                        break // terminate the program
                    },
                    signal_hook::SIGTERM => {
                        info!("SIGTERM was received");
                        tx.send(signal_hook::SIGTERM).unwrap();
                        break // terminate the program
                    },
                    _ => unreachable!(),
                }
            }

            // Wait until main thread was finished
            thread.join().expect_err("Thread.join()");
        }
        Err(e) => panic!("Error, {}", e),
    }
}
