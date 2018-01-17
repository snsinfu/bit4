provider "scaleway" {
  region = "par1"
}

resource "scaleway_server" "test" {
  name  = "test"
  image = "${data.scaleway_image.ubuntu.id}"
  type  = "VC1S"
}

data "scaleway_image" "ubuntu" {
  architecture = "x86_64"
  name         = "Ubuntu Xenial"
}

resource "scaleway_security_group" "web" {
  name        = "web"
  description = "Accepts incoming web packets"
}

resource "scaleway_security_group_rule" "http_accept" {
  security_group = "${scaleway_security_group.web.id}"

  action    = "accept"
  direction = "inbound"
  ip_range  = "0.0.0.0/0"
  protocol  = "TCP"
  port      = 80
}

resource "scaleway_security_group_rule" "https_accept" {
  security_group = "${scaleway_security_group.web.id}"

  action    = "accept"
  direction = "inbound"
  ip_range  = "0.0.0.0/0"
  protocol  = "TCP"
  port      = 443
}
