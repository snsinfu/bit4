resource "hcloud_server" "dokku" {
  name        = "dokku.example.com"
  location    = "nbg1"
  server_type = "cx11"
  image       = "debian-9"
  user_data   = "${file("cloud-init.yml")}"
  ssh_keys    = ["${hcloud_ssh_key.default.id}"]
}

resource "hcloud_ssh_key" "default" {
  name       = "default"
  public_key = "${file("ssh_key.pub")}"
}

resource "hcloud_volume" "dokku" {
  name      = "dokku"
  size      = 50
  server_id = "${hcloud_server.dokku.id}"
}

data "template_file" "hosts" {
  template = "${file("hosts.yml.tpl")}"

  vars {
    server_address = "${hcloud_server.dokku.ipv4_address}"
    dokku_device   = "${hcloud_volume.dokku.linux_device}"
  }
}

output "ip" {
  value = "${hcloud_server.dokku.ipv4_address}"
}

output "inventory" {
  value = "${data.template_file.hosts.rendered}"
}
