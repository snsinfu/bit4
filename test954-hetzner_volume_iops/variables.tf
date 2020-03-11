variable "server_location" {
  type    = string
  default = "fsn1"
}

variable "server_type" {
  type    = string
  default = "cx41"
}

variable "volume_size" {
  type    = number
  default = 50
}

variable "admin_user" {
  type    = string
  default = "hetzner"
}

variable "admin_pubkeys" {
  type = list(string)
}
