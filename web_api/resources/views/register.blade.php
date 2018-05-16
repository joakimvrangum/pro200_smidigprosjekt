@extends('layout.layout')

@section('content')

<h2>Registrer vare</h2>

<!-- FORM: REGISTRER NY VARE -->
<form id="formRegVare" method="POST" action="{{ url('vare/ny') }}" enctype="multipart/form-data">
	<div class="input-group">
	<span class="input-group-addon">Bilde:</span>
	<input type="file" accept="image/*" name="bilde"/>
	</div>
	<br>
	<input type="hidden" name="_token" value="{{ csrf_token() }}">
	<!-- KATEGORI - ikke i bruk enda -->
<!-- 	<div class="input-group">
	<span class="input-group-addon">Vare kategori:</span>
	<select name="kategori" class="form-control source">
		<option value="KAT1">Kategori #1</option>
		<option value="KAT2">Kategori #2</option>
	</select>
	</div> -->
	<!-- STREKKODE -->
	<div class="input-group">
		<span class="input-group-addon">Strekkode:</span>
		<input type="text" class="form-control" name="upc" placeholder="Scan strekkode">
	</div>
	<!-- NAVN -->
	<div class="input-group">
		<span class="input-group-addon">Navn</span>
		<input type="text" class="form-control" name="navn" placeholder="Navn på vare">
	</div>
	<!-- BESKRIVELSE -->
	<div class="input-group">
		<span class="input-group-addon">Beskrivelse:</span>
		<input type="text" class="form-control" name="beskrivelse" placeholder="Beskrivelse av varen">
	</div>
	<!-- PRIS -->
	<div class="input-group">
		<span class="input-group-addon">Pris:</span>
		<input type="text" class="form-control" name="pris" placeholder="Pris (f.eks 9.90)">
	</div>
	<br>
	<!-- KNAPPER -->
	<button type="submit" class="btn btn-warning" style="float:right; margin:2px;">Registrer</button>
	<button type="button" class="btn btn-warning" style="float:right; margin:2px;">Avbryt</button>
</form>

<!-- FEILMELDINGER -->
@if (count($errors) > 0)
	<div class="clearfix">
		<ul>
		@foreach ($errors->all() as $error)
		<li>{!! $error !!}</li>
		@endforeach
		</ul>
	</div>
@endif

@stop