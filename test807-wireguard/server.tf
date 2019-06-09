resource "digitalocean_droplet" "server" {
  name      = "${local.hostname}"
  region    = "sgp1"
  size      = "s-1vcpu-1gb"
  image     = "freebsd-12-x64"
  ssh_keys  = ["${digitalocean_ssh_key.default.fingerprint}"]
  user_data = "${data.template_file.cloudinit.rendered}"
}

resource "digitalocean_ssh_key" "default" {
  name       = "${local.hostname}-sshkey"
  public_key = "${file("_id_ed25519.pub")}"
}

data "template_file" "cloudinit" {
  template = "${file("tf_templates/cloudinit.yml.in")}"
  vars     = {
    admin_user  = "${var.admin_user}"
    ssh_pubkeys = "${jsonencode(var.ssh_pubkeys)}"
  }
}

# Outputs for subsequent provisionning

data "template_file" "ssh_config" {
  template = "${file("tf_templates/ssh_config.in")}"
  vars     = {
    host_address = "${digitalocean_droplet.server.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

data "template_file" "hosts" {
  template = "${file("tf_templates/hosts.in")}"
  vars     = {
    host_address = "${digitalocean_droplet.server.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

output "ip" {
  value = "${digitalocean_droplet.server.ipv4_address}"
}

output "ssh_config" {
  value     = "${data.template_file.ssh_config.rendered}"
  sensitive = true
}

output "hosts" {
  value     = "${data.template_file.hosts.rendered}"
  sensitive = true
}
