resource "vultr_instance" "srv" {
  name              = "srv.${local.domain}"
  hostname          = "srv.${local.domain}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.freebsd.id}"
  startup_script_id = "${vultr_startup_script.minimal.id}"
}

resource "vultr_instance" "pen" {
  name              = "pen.${local.domain}"
  hostname          = "pen.${local.domain}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.freebsd.id}"
  startup_script_id = "${vultr_startup_script.minimal.id}"
}

resource "vultr_startup_script" "minimal" {
  name    = "${local.domain}-minimal"
  content = "${data.template_file.startup_script.rendered}"
}

data "template_file" "startup_script" {
  template = "${file("${path.module}/templates/minimal.sh.in")}"
  vars = {
    admin_user    = "${var.admin_user}"
    admin_pubkeys = "${file("_id_ed25519.pub")}"
  }
}

# Outputs for subsequent provisioning.

data "template_file" "ssh_config" {
  template = "${file("${path.module}/templates/ssh_config.in")}"
  vars     = {
    srv_address = "${vultr_instance.srv.ipv4_address}"
    pen_address = "${vultr_instance.pen.ipv4_address}"
    admin_user  = "${var.admin_user}"
  }
}

data "template_file" "ansible_inventory" {
  template = "${file("${path.module}/templates/hosts.in")}"
  vars     = {
    srv_address = "${vultr_instance.srv.ipv4_address}"
    pen_address = "${vultr_instance.pen.ipv4_address}"
    admin_user  = "${var.admin_user}"
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
