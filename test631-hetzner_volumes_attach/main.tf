variable "location" {
  default = "hel1"
}

resource "hcloud_ssh_key" "primary" {
  name       = "primary key"
  public_key = "${file("~/.ssh/id_rsa.pub")}"
}

resource "hcloud_server" "main" {
  name        = "main"
  image       = "debian-9"
  server_type = "cx11"
  location    = "${var.location}"
  ssh_keys    = ["${hcloud_ssh_key.primary.id}"]
}

resource "hcloud_volume" "primary" {
  name     = "primary_volume"
  size     = 50
  location = "${var.location}"
}

output "ip" {
  value = "${hcloud_server.main.ipv4_address}"
}

output "server" {
  value = "${hcloud_server.main.id}"
}

output "volume" {
  value = "${hcloud_volume.primary.id}"
}

output "volume_device" {
  value = "${hcloud_volume.primary.linux_device}"
}
