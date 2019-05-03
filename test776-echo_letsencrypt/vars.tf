variable "server_location" {
  default = "nbg1"
}

variable "server_type" {
  default = "cx11"
}

variable "server_image" {
  default = "debian-9"
}

variable "domain" {
}

variable "primary_pubkey" {
}

variable "admin_user" {
}

variable "email" {
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
