{ program 4.5
  compute the cosine using the expansion:
    cos(x) = 1 - x**2/(2*1) + x**4/(4*3*2*1) - ... }

program cosine(input, output);

const
  eps = 1e-14;

var
  x, sx, s, t : real;
  i, k, n : integer;

begin
  read(n);
  for i:=1 to n do
  begin
    read(x);
    t := 1; k := 0; s := 1; sx := sqr(x);
    while abs(t) > eps*abs(s) do
    begin
      k := k+2;
      t := -t*sx/(k*(k-1));
      s := s+t;
    wnd;  { error here! }
    writeln(x,s,k div 2)
  end
end. {SYNTAX ERROR HERE: unbalanced end}
