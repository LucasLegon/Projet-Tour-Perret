import { NgModule }             from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { CommonModule } from '@angular/common';

import { EditorComponent }   from './editor/editor.component';
import { HomeComponent }      from './home/home.component';
import { AproposComponent } from './apropos/apropos.component';
import { TutoComponent } from './tuto/tuto.component';

const routes: Routes = [
  { path: '', redirectTo: '/home', pathMatch: 'full' },
  { path: 'editor', component: EditorComponent },
  { path: 'home', component: HomeComponent },
  { path: 'a-propos', component: AproposComponent },
  { path: 'tuto', component: TutoComponent }
];

@NgModule({
  imports: [ RouterModule.forRoot(routes) ],
  exports: [ RouterModule ]
})
export class AppRoutingModule {}
