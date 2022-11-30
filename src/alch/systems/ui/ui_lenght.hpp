#pragma once

namespace al {

class UILenght {
  public:
	enum Units {
		PIXELS,
		PARTS,
	};

	operator float() const { return _lenghtInPixels; }
	// auto& operator=(float newV) {
	// 	lenght(newV);

	// 	return *this;
	// }

	float lenght() const { return _lenghtInPixels; }
	float lenghtInUnits() const { return _lenght; }

	float lenght(float newV) {
		_units = PIXELS;
		_lenghtInPixels = newV;

		return *this;
	}
	void lenghtInUnits(float newV) {
		if (_units == PARTS) {
			if (_lenght > 0) {
				_lenghtInPixels *= newV / _lenght;
			}
			_lenght = newV;
		} else {
			_lenghtInPixels = newV;
		}
	}

	void update(float mainPartSize) {
		if (_units == PARTS) {
			_lenghtInPixels = mainPartSize * _lenght;
		}
	}

	// auto& operator+=(const UILenght& o) {
	// 	if (_units == PIXELS) {
	// 		_lenghtInPixels += o._lenghtInPixels;
	// 	} else if (_units == PARTS) {
	// 		if (o._units == PIXELS) {
	// 			if (_lenght > 0) {
	// 				auto newPix = _lenghtInPixels + o._lenghtInPixels;
	// 				_lenght *= newPix / _lenghtInPixels;
	// 				_lenghtInPixels = newPix;
	// 			} else {
	// 				// тут непонятно что делать, поэтому меням систему счисления
	// 				_lenght = o._lenght;
	// 				_lenghtInPixels = o._lenghtInPixels;
	// 				_units = PIXELS;
	// 			}
	// 		} else if (o._units == PARTS) {
	// 			if (_lenght > 0) {
	// 				auto newPix = _lenghtInPixels + o._lenghtInPixels;
	// 				_lenght *= newPix / _lenghtInPixels;
	// 				_lenghtInPixels = newPix;
	// 			} else {
	// 				// тут непонятно что делать, поэтому меням систему счисления
	// 				_lenght = o._lenght;
	// 				_lenghtInPixels = o._lenghtInPixels;
	// 			}
	// 		}
	// 	}

	// 	return *this;
	// }

  private:
	Units _units = PIXELS;
	float _lenght;
	float _lenghtInPixels;
};

} // namespace al
