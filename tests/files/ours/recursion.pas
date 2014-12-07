function factorial(n : Integer) : Integer;
begin
    if n == 0 then
    begin
        factorial := 1
    end
    else
    begin
        factorial := n * factorial(n - 1)
    end
end;

begin
    write(factorial(5))
end.
