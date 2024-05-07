package com.khopan.camera;

import java.util.function.Supplier;

import com.khopan.camera.decoder.MJPGDecoder;

public enum VideoFormat {
	MJPG(() -> new MJPGDecoder()),
	YUY2(null),
	NV12(null),
	UNKNOWN(null);

	private final Supplier<Decoder> decoderSupplier;

	VideoFormat(Supplier<Decoder> decoderSupplier) {
		this.decoderSupplier = decoderSupplier;
	}

	public Supplier<Decoder> getDecoderSupplier() {
		if(VideoFormat.UNKNOWN.equals(this)) {
			throw new RuntimeException("Unknown type");
		}

		return this.decoderSupplier;
	}

	public Decoder newDecoder() {
		if(VideoFormat.UNKNOWN.equals(this)) {
			throw new RuntimeException("Unknown type");
		}

		return this.decoderSupplier.get();
	}
}
