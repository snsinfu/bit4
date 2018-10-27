resource "hcloud_ssh_key" "primary" {
  name       = "primary key"
  public_key = "${file("~/.ssh/id_rsa.pub")}"
}

resource "hcloud_server" "main" {
  name        = "main"
  image       = "debian-9"
  server_type = "cx11"
  location    = "hel1"
  ssh_keys    = ["${hcloud_ssh_key.primary.id}"]
}

resource "hcloud_volume" "primary" {
  name      = "primary_volume"
  size      = 50
  server    = "${hcloud_server.main.id}"
}

output "ip" {
  value = "${hcloud_server.main.ipv4_address}"
}

output "volume" {
  value = "${hcloud_volume.primary.linux_device}"
}
