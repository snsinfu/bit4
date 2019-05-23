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

resource "vultr_instance" "server" {
  name              = "${local.hostname}"
  hostname          = "${local.hostname}"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.freebsd.id}"
  startup_script_id = "${vultr_startup_script.server.id}"
  firewall_group_id = "${vultr_firewall_group.server.id}"
}

resource "vultr_startup_script" "server" {
  name    = "${local.hostname} startup script"
  content = "${data.template_file.startup_script.rendered}"
}

data "template_file" "startup_script" {
  template = "${file("startup.sh.in")}"
  vars {
    admin_user  = "${var.admin_user}"
    ssh_pubkeys = "${join("\n", var.ssh_pubkeys)}"
  }
}

resource "vultr_firewall_group" "server" {
  description = "server firewall"
}

resource "vultr_firewall_rule" "ssh" {
  firewall_group_id = "${vultr_firewall_group.server.id}"
  cidr_block        = "0.0.0.0/0"
  protocol          = "tcp"
  from_port         = 22
  to_port           = 22
}

resource "vultr_firewall_rule" "openvpn" {
  firewall_group_id = "${vultr_firewall_group.server.id}"
  cidr_block        = "0.0.0.0/0"
  protocol          = "udp"
  from_port         = 974
  to_port           = 974
}

# Outputs for subsequent provisioning

data "template_file" "ssh_config" {
  template = "${file("ssh_config.in")}"
  vars {
    host_address = "${vultr_instance.server.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

data "template_file" "hosts" {
  template = "${file("hosts.in")}"
  vars {
    host_address = "${vultr_instance.server.ipv4_address}"
    admin_user   = "${var.admin_user}"
  }
}

output "ip" {
  value = "${vultr_instance.server.ipv4_address}"
}

output "ssh_config" {
  value     = "${data.template_file.ssh_config.rendered}"
  sensitive = true
}

output "hosts" {
  value     = "${data.template_file.hosts.rendered}"
  sensitive = true
}
