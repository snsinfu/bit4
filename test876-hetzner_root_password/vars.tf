variable "admin_user" {
  type = "string"
}

variable "admin_pubkeys" {
  type = "list"
}

variable "server_location" {
  type    = "string"
  default = "nbg1"
}

variable "server_type" {
  type    = "string"
  default = "cx11"
}

variable "server_image" {
  type    = "string"
  default = "ubuntu-18.04"
}

resource "random_id" "hostid" {
  byte_length = 4
}

locals {
  hostname = "test-${random_id.hostid.hex}"
}
