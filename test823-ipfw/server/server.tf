resource "vultr_instance" "main" {
  name              = "${local.hostname}"
  hostname          = "${local.hostname}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.freebsd.id}"
  startup_script_id = "${vultr_startup_script.main.id}"
}

data "vultr_region" "tokyo" {
  filter {
    name   = "name"
    values = ["Tokyo"]
  }
}

data "vultr_plan" "starter" {
  filter {
    name   = "price_per_month"
    values = ["5.00"]
  }
}

data "vultr_os" "freebsd" {
  filter {
    name   = "name"
    values = ["FreeBSD 12 x64"]
  }
}

resource "vultr_startup_script" "main" {
  name    = "main"
  content = "${data.template_file.startup_script.rendered}"
}

data "template_file" "startup_script" {
  template = "${file("${path.module}/templates/startup.sh.in")}"
  vars = {
    admin_user    = "${var.admin_user}"
    admin_pubkeys = "${file("_id_ed25519.pub")}"
  }
}

# Outputs for subsequent provisioning.

data "template_file" "ssh_config" {
  template = "${file("${path.module}/templates/ssh_config.in")}"
  vars     = {
    host_address = "${vultr_instance.main.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

data "template_file" "ansible_inventory" {
  template = "${file("${path.module}/templates/hosts.in")}"
  vars     = {
    host_address = "${vultr_instance.main.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

output "ip" {
  value = "${vultr_instance.main.ipv4_address}"
}

output "ssh_config" {
  value     = "${data.template_file.ssh_config.rendered}"
  sensitive = true
}

output "ansible_inventory" {
  value     = "${data.template_file.ansible_inventory.rendered}"
  sensitive = true
}
