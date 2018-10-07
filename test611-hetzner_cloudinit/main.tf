resource "hcloud_ssh_key" "tryout" {
  name       = "tryout key"
  public_key = "${file("id_rsa.pub")}"
}

resource "hcloud_server" "tryout" {
  name        = "tryout.example.com"
  location    = "${var.location}"
  server_type = "${var.type}"
  image       = "${var.os}"
  ssh_keys    = ["${hcloud_ssh_key.tryout.id}"]
  user_data   = "${file("user_data.cfg")}"
}

output "ip" {
  value = "${hcloud_server.tryout.ipv4_address}"
}
