variable "admin_user" {
  type = string
}

variable "admin_pubkeys" {
  type = list(string)
}

variable "server_hostname" {
  type    = string
  default = "main"
}

variable "server_type" {
  type    = string
  default = "cx11"
}

variable "server_location" {
  type    = string
  default = "fsn1"
}

variable "server_image" {
  type    = string
  default = "debian-10"
}
