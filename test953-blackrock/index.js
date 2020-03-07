"use strict";

import { outerJoin } from "./utils/algorithm.js";
import { strictEval } from "./utils/eval.js";
import { requestHTTPS } from "./utils/request.js"


const BASE_URL = "https://www.blackrock.com/jp/individual/ja/products"

async function main(args) {
  if (args.length != 1) {
    console.error("usage: node index.js <portfolio-id>");
    process.exit(1);
  }

  const portfolio_id = args[0];
  const url = BASE_URL + "/" + portfolio_id + "/";

  const doc = await requestHTTPS(url);
  const [ , navDataJS ] = doc.match(/^var navData = (\[.*\]);$/m);
  const [ , priceDataJS ] = doc.match(/^var priceData = (\[.*\]);$/m);
  const navData = strictEval(navDataJS);
  const priceData = strictEval(priceDataJS);

  const navPriceData = outerJoin(
    navData, priceData, (n, p) => (n.x > p.x) - (n.x < p.x)
  );

  console.log("date\tnav\tprice");

  navPriceData.forEach(row => {
    const [ navRow, priceRow ] = row;
    const date = new Date(navRow ? navRow.x : priceRow.x);
    const nav = navRow ? navRow.y : "#N/A";
    const price = priceRow ? priceRow.y : "#N/A";
    const dateString = date.toISOString().slice(0, 10);

    console.log(`${dateString}\t${nav}\t${price}`);
  });
}


main(process.argv.slice(2));
