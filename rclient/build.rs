extern crate protoc_rust;

fn main() {
    protoc_rust::run(protoc_rust::Args {
        out_dir: "src",
        includes: &["../"],
        input: &["../3rdparty/proto/falcon.proto"],
        customize: Default::default(),
    }).expect("protoc");
}
