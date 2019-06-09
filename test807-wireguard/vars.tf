variable "admin_user" {
  type    = "string"
  default = "ansible"
}

variable "ssh_pubkeys" {
  type    = "list"
  default = []
}

resource "random_id" "random_id" {
  byte_length = 4
}

locals {
  hostname = "test-${random_id.random_id.hex}"
}

output "hostname" {
  value = "${local.hostname}"
}
