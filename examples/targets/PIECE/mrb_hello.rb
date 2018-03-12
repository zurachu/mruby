def prime?(n)
  (2...n).each do |i|
    return false if n % i == 0
  end
  true
end

a = (2..100).select {|n| prime?(n) }
Pce::Font::put_str "#{a.to_s}\n"
