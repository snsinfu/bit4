"use strict";

export function strictEval(expr) {
  return Function(`"use strict"; return (${expr});`)();
}
