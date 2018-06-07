<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Kunde extends Model
{
	protected $table = 'kunder';
	protected $fillable = ['epost', 'navn', 'adresse'];
	protected $primaryKey = 'id';
}
