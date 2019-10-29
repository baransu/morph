let map_or = (~default: 'b, fn: 'a => 'b, opt: option('a)) =>
  switch (opt) {
  | Some(value) => fn(value)
  | None => default
  };

let map_opt = (fn: 'a => 'b, opt: option('a)) =>
  switch(opt) {
    | None => None
    | Some(v) => Some(fn(v))
  };
