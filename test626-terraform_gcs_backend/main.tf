terraform {
  backend "gcs" {
    bucket      = "terraform-fd1f70508ba7e3d079fa3b4e8bb05f0a"
    prefix      = "state"
    credentials = "credentials.json"
  }
}

resource "random_id" "main" {
  byte_length = 8
}

output "id" {
  value = "${random_id.main.hex}"
}
