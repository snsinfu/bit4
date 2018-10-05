resource "hcloud_ssh_key" "primary" {
  name       = "primary key"
  public_key = "${file("id_rsa.pub")}"
}

resource "hcloud_server" "main" {
  name        = "main"
  server_type = "cx11"
  image       = "debian-9"
  location    = "hel1"
  ssh_keys    = ["${hcloud_ssh_key.primary.id}"]
}

data "template_file" "hosts" {
  template = "${file("hosts.ini.tpl")}"

  vars {
    server_address = "${hcloud_server.main.ipv4_address}"
  }
}

output "ip" {
  value = "${hcloud_server.main.ipv4_address}"
}

output "inventory" {
  value = "${data.template_file.hosts.rendered}"
}
