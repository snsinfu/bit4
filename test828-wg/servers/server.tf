resource "vultr_instance" "relay" {
  name              = "relay.${local.domain}"
  hostname          = "relay.${local.domain}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.ubuntu.id}"
  startup_script_id = "${vultr_startup_script.minimal.id}"
}

resource "vultr_instance" "gateway" {
  name              = "gateway.${local.domain}"
  hostname          = "gateway.${local.domain}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.ubuntu.id}"
  startup_script_id = "${vultr_startup_script.minimal.id}"
}

# Outputs for subsequent provisioning.

data "template_file" "ssh_config" {
  template = "${file("${path.module}/templates/ssh_config.in")}"
  vars     = {
    relay_address   = "${vultr_instance.relay.ipv4_address}"
    gateway_address = "${vultr_instance.gateway.ipv4_address}"
    admin_user      = "${var.admin_user}"
  }
}

data "template_file" "ansible_inventory" {
  template = "${file("${path.module}/templates/hosts.in")}"
  vars     = {
    relay_address   = "${vultr_instance.relay.ipv4_address}"
    gateway_address = "${vultr_instance.gateway.ipv4_address}"
    admin_user      = "${var.admin_user}"
  }
}

output "ssh_config" {
  value     = "${data.template_file.ssh_config.rendered}"
  sensitive = true
}

output "ansible_inventory" {
  value     = "${data.template_file.ansible_inventory.rendered}"
  sensitive = true
}
