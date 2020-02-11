variable "server_hostname" {
  type    = string
  default = "freebsd"
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

variable "server_iso" {
  type    = string
  default = "FreeBSD-12.1-RELEASE-amd64-bootonly.iso"
}
