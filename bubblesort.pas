program BubbleSort;

var
  arr: array[1..10] of integer;
  i, j, temp: integer;

begin
  writeln('Enter 10 numbers:');
  for i := 1 to 10 do
    readln(arr[i]);

  for i := 1 to 9 do
  begin
    for j := 1 to 10-i do
    begin
      if arr[j] > arr[j+1] then
      begin
        temp := arr[j];
        arr[j] := arr[j+1];
        arr[j+1] := temp;
      end;
    end;
  end;

  writeln('Sorted array:');
  for i := 1 to 10 do
    writeln(arr[i]);
end.
