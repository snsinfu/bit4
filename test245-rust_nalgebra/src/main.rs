extern crate nalgebra as na;

use na::Point3 as Point;

fn main() {
    let p = Point::new(1.2, 3.4, 5.6);
    let q = Point::new(9.1, 8.2, 7.3);
    let r = p - q;
    let f = r / -na::norm_squared(&r);

    println!("{}\t{}\t{}", f[0], f[1], f[2]);
}
