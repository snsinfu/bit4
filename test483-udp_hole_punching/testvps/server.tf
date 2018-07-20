#
# Machine
#

resource "vultr_instance" "primary" {
  name              = "test483"
  hostname          = "test483"
  region_id         = "${data.vultr_region.tokyo.id}"
  plan_id           = "${data.vultr_plan.starter.id}"
  os_id             = "${data.vultr_os.freebsd.id}"
  ssh_key_ids       = ["${vultr_ssh_key.primary.id}"]
  firewall_group_id = "${vultr_firewall_group.primary.id}"
}

output "ip" {
  value = "${vultr_instance.primary.ipv4_address}"
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
  filter {
    name   = "ram"
    values = ["1024"]
  }
}

data "vultr_os" "freebsd" {
  filter {
    name   = "name"
    values = ["FreeBSD 11 x64"]
  }
}

#
# Key
#

resource "vultr_ssh_key" "primary" {
  name       = "test483"
  public_key = "${file("ssh_key.pub")}"
}

#
# Security
#

resource "vultr_firewall_group" "primary" {
  description = "primary firewall group"
}

resource "vultr_firewall_rule" "ssh" {
  firewall_group_id = "${vultr_firewall_group.primary.id}"
  cidr_block        = "0.0.0.0/0"
  protocol          = "tcp"
  from_port         = 22
  to_port           = 22
}
