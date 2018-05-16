<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Handlekurv extends Model
{
	protected $table = 'handlekurv';

	public function varedata() {
		return $this->hasOne('App\Vare', 'upc', 'upc');
	}
}
