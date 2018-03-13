def prime?(n)
  (2...n).each do |i|
    return false if n % i == 0
  end
  true
end

def puts(x)
  Pce::Font::put_str "#{x}\n"
end

a = (2..100).select {|n| prime?(n) }
puts a
