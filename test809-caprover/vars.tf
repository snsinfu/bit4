variable "server_location" {
  default = "nbg1"
}

variable "server_type" {
  default = "cx11"
}

variable "server_image" {
  default = "ubuntu-18.04"
}

variable "domain" {
}

variable "primary_pubkey" {
}

variable "admin_user" {
}

resource "random_id" "subdomain_id" {
  byte_length = 4
}

locals {
  subdomain = "test-${random_id.subdomain_id.hex}"
  hostname  = "${local.subdomain}.${var.domain}"
}

output "hostname" {
  value = "${local.hostname}"
}
