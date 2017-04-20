Rails.application.routes.draw do
  get 'ubications/index'
  get 'variable/index'
  root 'ubications#index'

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
