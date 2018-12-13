locals {
  server_address = "${google_compute_instance.main.network_interface.0.access_config.0.nat_ip}"
}

data "template_file" "ssh_config" {
  template = "${file("ssh_config.in")}"

  vars {
    server_address = "${local.server_address}"
    ssh_user       = "${var.ssh_user}"
  }
}

output "ip" {
  value = "${local.server_address}"
}

output "ssh_config" {
  value = "${data.template_file.ssh_config.rendered}"
}
