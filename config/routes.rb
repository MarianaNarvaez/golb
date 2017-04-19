Blog::Application.routes.draw do
#Rails.application.routes.draw do
  get 'welcome/index'
  get 'articles/new'
  resources :articles
  root 'welcome#index'
  resources :articles do
  	resources :comments
  end
end
