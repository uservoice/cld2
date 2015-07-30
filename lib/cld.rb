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
  
  def self.detect_language(text, verbose=false, is_plain_text=true)
    result = detect_language_ext(text.to_s, is_plain_text)
    hash = Hash[ result.members.
      select {|member| ["name", "code", "reliable"].include? member.to_s}.
      map {|member| [member.to_sym, result[member]]} ]

    if verbose
      hash[:top_langs] = get_top_languages(result[:lang_results_ptr])
      hash[:chunks] = get_chunk_results(result[:chunks_results_ptr], result[:num_chunks], text)
    end

    hash
  end

  def self.get_top_languages(lang_results_ptr)
    lang_arr = []
    0.upto(2) do |i|
      lang_result = LanguageResult.new(lang_results_ptr + (i * LanguageResult.size))
      if !lang_result[:score].zero?
        lang_arr << Hash[ lang_result.members. map {|member| [member.to_sym, lang_result[member]]} ]
      end
    end
    lang_arr
  end

  def self.get_chunk_results(chunks_results_ptr, num_chunks, text)
    chunks_arr = []
    0.upto(num_chunks - 1) do |i|
      chunk = Chunk.new(chunks_results_ptr + (i * Chunk.size))
      chunks_arr << {
        content: text.byteslice(chunk[:offset], chunk[:bytes]),
        code: chunk[:code],
      }
    end
    chunks_arr
  end

  private

  class ReturnValue < FFI::Struct
    layout  :name, :string, :code, :string, :reliable, :bool,
            :lang_results_ptr, :pointer,
            :num_chunks, :int, :chunks_results_ptr, :pointer
  end
  
  class LanguageResult < FFI::Struct
    layout  :code, :string, :percent, :int, :score, :double
  end

  class Chunk < FFI::Struct
    layout  :offset, :int, :bytes, :uint16,
            :code, :string
  end

  attach_function "detect_language_ext", "detectLanguageThunkInt", [:buffer_in, :bool], ReturnValue.by_value
end
