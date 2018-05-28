resource "hcloud_ssh_key" "primary" {
  name       = "primary key"
  public_key = "${file("id_rsa.pub")}"
}

resource "hcloud_server" "builder" {
  name        = "builder"
  server_type = "cx11"
  image       = "ubuntu-18.04"
  location    = "fsn1"
  ssh_keys    = ["${hcloud_ssh_key.primary.id}"]
}

output "ip" {
  value = "${hcloud_server.builder.ipv4_address}"
}
