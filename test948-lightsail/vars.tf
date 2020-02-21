variable "server_zone" {
  type = string
}

variable "server_blueprint" {
  type = string
}

variable "server_bundle" {
  type = string
}

variable "admin_user" {
  type = string
}

variable "admin_password" {
  type = string
}

variable "admin_pubkeys" {
  type = list(string)
}
