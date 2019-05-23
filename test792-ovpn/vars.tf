variable "admin_user" {
  type = "string"
}

variable "ssh_pubkeys" {
  type = "list"
}

resource "random_id" "random_id" {
  byte_length = 4
}

locals {
  hostname  = "test-${random_id.random_id.hex}"
}

output "hostname" {
  value = "${local.hostname}"
}
