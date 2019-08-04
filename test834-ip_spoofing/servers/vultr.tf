data "vultr_region" "tokyo" {
  filter {
    name   = "name"
    values = ["Tokyo"]
  }
}

data "vultr_plan" "starter" {
  filter {
    name   = "price_per_month"
    values = ["5.00"]
  }
}

data "vultr_os" "freebsd" {
  filter {
    name   = "name"
    values = ["FreeBSD 12 x64"]
  }
}
