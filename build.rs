extern crate bindgen;
extern crate dotenv;

use std::env;
use std::path::PathBuf;

fn main() {
    dotenv::dotenv().ok();

    println!("start build");
    // let headers_dir: Vec<_> =
    //     env::var("INCLUDE_DIR")
    //         .unwrap_or("".to_string())
    //         .split(' ')
    //         .map(|v| {
    //             let str = format!("-F{}", v);
    //             str
    //         })
    //         .collect();

    let bindings = bindgen::Builder::default()
        // .clang_args(headers_dir)
        .header("out/test.h")
        .rust_target(bindgen::RustTarget::Stable_1_40)
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUTPUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("binding.rs"))
        .expect("Couldn't write bindings!");
}
