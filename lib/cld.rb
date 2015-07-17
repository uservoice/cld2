# coding: utf-8

require "cld/version"
require "ffi"

module CLD
  extend FFI::Library

  # Workaround FFI dylib/bundle issue.  See https://github.com/ffi/ffi/issues/42
  suffix = if FFI::Platform.mac?
    'bundle'
  else
    FFI::Platform::LIBSUFFIX
  end

  ffi_lib File.join(File.expand_path(File.dirname(__FILE__)), '..', 'ext', 'cld', 'libcld2.' + suffix)
  
  def self.detect_language(text, is_plain_text=true)
    result = detect_language_ext(text.to_s, is_plain_text)
    
    hash = Hash[ result.members.
      select {|member| !["num_chunks","chunks_array"].include? member.to_s}.
      map {|member| [member.to_sym, result[member]]} ]

    hash[:chunks] = []

    0.upto(result[:num_chunks] - 1) do |i|
      chunk = Chunk.new(result[:chunks_array] + (i * Chunk.size))
      hash[:chunks] << Hash[ chunk.members.map {|member| [member.to_sym, chunk[member]]} ]
    end

    hash
  end

  private

  class ReturnValue < FFI::Struct
    layout  :name, :string, :code, :string, :reliable, :bool, 
            :num_chunks, :int, :chunks_array, :pointer
  end
  
  class Chunk < FFI::Struct
    layout  :offset, :int, :bytes, :uint16, 
            :code, :string
  end

  attach_function "detect_language_ext", "detectLanguageThunkInt", [:buffer_in, :bool], ReturnValue.by_value
end
