package com.khopan.media.camera;

import java.util.function.Supplier;

import com.khopan.media.camera.decoder.MJPGDecoder;
import com.khopan.media.camera.decoder.NV12Decoder;
import com.khopan.media.camera.decoder.YUY2Decoder;

public enum VideoFormat {
	MJPG(() -> new MJPGDecoder()),
	YUY2(() -> new YUY2Decoder()),
	NV12(() -> new NV12Decoder()),
	UNKNOWN(null);

	private final Supplier<Decoder> decoderSupplier;

	VideoFormat(Supplier<Decoder> decoderSupplier) {
		this.decoderSupplier = decoderSupplier;
	}

	public Supplier<Decoder> getDecoderSupplier() {
		if(VideoFormat.UNKNOWN.equals(this)) {
			throw new RuntimeException("Unknown type");
		}

		if(this.decoderSupplier == null) {
			throw new RuntimeException("Unsupported decoder");
		}

		return this.decoderSupplier;
	}

	public Decoder newDecoder() {
		return this.getDecoderSupplier().get();
	}
}
