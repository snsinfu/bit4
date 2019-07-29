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

data "vultr_os" "ubuntu" {
  filter {
    name   = "name"
    values = ["Ubuntu 18.04 x64"]
  }
}

resource "vultr_startup_script" "minimal" {
  name    = "${local.domain}-minimal"
  content = "${data.template_file.startup_script_main.rendered}"
}

data "template_file" "startup_script_main" {
  template = "${file("${path.module}/templates/minimal.sh.in")}"
  vars = {
    admin_user    = "${var.admin_user}"
    admin_pubkeys = "${file("_id_ed25519.pub")}"
  }
}
