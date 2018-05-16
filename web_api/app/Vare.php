<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Vare extends Model
{
	protected $table = 'varer';
	protected $fillable = ['upc', 'vare_navn', 'vare_beskrivelse', 'pris'];
	protected $primaryKey = 'upc';
}
