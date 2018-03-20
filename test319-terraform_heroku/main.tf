provider "heroku" {
}

resource "heroku_app" "app" {
  name = "app-from-terraform"
  region = "us"

  config_vars = {
    MESSAGE = "Terraform!"
  }

  buildpacks = [
    "heroku/go"
  ]
}

resource "heroku_addon" "database" {
  app = "${heroku_app.app.name}"
  plan = "heroku-postgresql:hobby-dev"
}

output "git" {
  value = "${heroku_app.app.git_url}"
}

output "web" {
  value = "${heroku_app.app.web_url}"
}
