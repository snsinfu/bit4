variable "server_hostname" {
  type    = string
  default = "test.example.com"
}

variable "server_location" {
  type    = string
  default = "nbg1"
}

variable "server_type" {
  type    = string
  default = "cx11"
}

variable "admin_user" {
  type    = string
  default = "hetzner"
}

variable "admin_password" {
  type = string
}

variable "admin_pubkeys" {
  type = list(string)
}
