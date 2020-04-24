variable "server_hostname" {
  type    = string
  default = "test960.example.com"
}

variable "server_location" {
  type    = string
  default = "fsn1"
}

variable "server_type" {
  type    = string
  default = "cx11"
}

variable "server_image" {
  type = string
}

variable "volume_name" {
  type    = string
  default = "test960"
}

variable "volume_size" {
  type    = number
  default = 50
}

variable "admin_user" {
  type = string
}

variable "admin_public_keys" {
  type = list(string)
}
