variable "admin_user" {
  type = string
}

variable "admin_pubkeys" {
  type = list
}

variable "server_location" {
  type    = string
  default = "nbg1"
}

variable "server_type" {
  type    = string
  default = "cx11"
}

variable "server_image" {
  type    = string
  default = "debian-10"
}
